import requests
from datetime import datetime, timedelta
import threading
from DataSource import DataSource
from time import sleep

class TiingoApi(DataSource):
    def __init__(self, token):
        self.token = token
        self.url = "https://api.tiingo.com"
        self.subDir = ""
        self.symbol = ""
        self.rep = ""
        self.dfr = False
        self.sampleFreq = 10
        self.historyLen = 30
        self.manager = requests.Session()
        self.manager.headers.update({'Content-Type': 'application/json'})

        self.test_connection()

    def test_connection(self):
        uri = self.url + "/api/test?token=" + self.token
        response = self.manager.get(uri)
        if "success" in response.json()['message']:
            print("Success")
        else:
            print("Failed connection to server: " + str(response.json()))
            exit(0)

    def set_ticker_symbol(self, symbol, params):
        self.symbol = symbol
        if self.subDir == "" and len(params) < 1:
            print("params required")
            return False
        else:
            self.subDir = params[0]

        # test connection
        # set uri and send request
        uri = self.url + self.subDir + "prices?tickers=" + self.symbol + "&token=" + self.token
        response = self.manager.get(uri)
        if len(str(response.json())) >= 30:
            print("success")
            return True
        else:
            print("failed")
            return False

    def set_sample_freq(self, min):
        self.sampleFreq = min

    def set_history_len(self, days):
        self.historyLen = days

    def data_updated(self):
        return self.dfr

    def get_data_frame(self):
        return None

    def get_current_price(self):
        return 0

    def read_reply(self, reply):
        self.rep = reply.content.decode("utf-8")
        self.dr = True

    def start_live(self):
        t = threading.Thread(target=self.thread)
        t.start()

    def thread(self):
        curr_t = int(datetime.utcnow().timestamp())
        # get history data before current time
        man = requests.Session()
        man.headers.update({'Content-Type': 'application/json'})
        self.get_history_data(curr_t)

    def get_history_data(self, curr_t):
        gmtm = datetime.utcfromtimestamp(curr_t)
        # get the current time
        curr_sec = gmtm.second
        curr_min = gmtm.minute
        curr_hr = gmtm.hour
        # use the current time to offset the time to 0:0:0 UTC current date
        offset_t = curr_t - curr_sec - curr_min * 60 - curr_hr * 3600
        start_t = curr_t - self.historyLen * 86400
        startDate = self.date_conversion(offset_t, self.historyLen)
        # query maximum days with <=5000 items at a time until the current date at 0:0:0 UTC
        maxQueryDays = self.sampleFreq * 5000 // (24 * 60)
        print("maxDays: ", maxQueryDays, " max per request: ", (maxQueryDays * 24 * 60) // self.sampleFreq)
        for queryStartDate in range(start_t, offset_t, maxQueryDays * 86400):
            queryEndDate = queryStartDate + maxQueryDays * 86400
            if queryEndDate > offset_t:
                queryEndDate = offset_t
            startDateStr = self.date_conversion(queryStartDate, 0)
            endDateStr = self.date_conversion(queryEndDate, 0)
            print("QueryDate:" + startDateStr + "--" + endDateStr)
            uri = self.url + self.subDir + "prices?startDate=" + startDateStr + "&endDate=" + endDateStr + "&resampleFreq=" + str(
                self.sampleFreq) + "min&tickers=" + self.symbol + "&token=" + self.token
            response = self.manager.get(uri)
            print(len(response.json()[0]["priceData"]))

    def date_conversion(self, t, delta):
        gmtm = datetime.utcfromtimestamp(t - delta * 86400)
        yr = str(gmtm.year)
        mn = str(gmtm.month).zfill(2)
        dy = str(gmtm.day).zfill(2)
        return yr + "-" + mn + "-" + dy