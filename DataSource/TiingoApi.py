import requests
from datetime import datetime, timedelta
import threading
from DataSource import DataSource
from time import sleep
import pandas as pd


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
        self.df = pd.DataFrame()
        self.test_connection()

    def test_connection(self):
        uri = self.url + "/api/test?token=" + self.token
        try:
            response = self.manager.get(uri)
        except requests.exceptions.RequestException:
            print("connection failed")
            exit(0)
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
        return self.df

    def get_current_price(self):
        return 0

    def read_reply(self, reply):
        self.rep = reply.content.decode("utf-8")

    def start_live(self):
        t = threading.Thread(target=self.thread)
        t.start()

    def thread(self):
        curr_t = int(datetime.utcnow().timestamp())
        # get history data before current time
        man = requests.Session()
        man.headers.update({'Content-Type': 'application/json'})
        self.get_history_data(curr_t)

    # get history data until 0:0:0 UTC today
    def get_history_data(self, curr_t):
        gmtm = datetime.utcfromtimestamp(curr_t)
        # get the current time
        curr_sec = gmtm.second
        curr_min = gmtm.minute
        curr_hr = gmtm.hour
        # use the current time to offset the time to 0:0:0 UTC current date
        offset_t = curr_t - curr_sec - curr_min * 60 - curr_hr * 3600
        start_t = curr_t - self.historyLen * 86400
        # query maximum days with <=5000 items at a time until the current date at 0:0:0 UTC
        max_query_days = self.sampleFreq * 5000 // (24 * 60)
        print("maxDays: ", max_query_days, " max per request: ", (max_query_days * 24 * 60) // self.sampleFreq)
        first = True;
        for query_start_date in range(start_t, offset_t, max_query_days * 86400):
            query_end_date = query_start_date + max_query_days * 86400
            if query_end_date > offset_t:
                query_end_date = offset_t
            start_date_str = self.date_conversion(query_start_date, 0)
            end_date_str = self.date_conversion(query_end_date, 0)
            print("QueryDate:" + start_date_str + "--" + end_date_str)
            uri = self.url + self.subDir + "prices?startDate=" + start_date_str + "&endDate=" + end_date_str + \
                  "&resampleFreq=" + str(self.sampleFreq) + "min&tickers=" + self.symbol + "&token=" + self.token
            response = self.manager.get(uri)
            print(len(response.json()[0]["priceData"]))
            if first:
                self.parseData(response.json()[0]["priceData"])
                first = False
            else:
                self.parseData(response.json()[0]["priceData"][1:])
            self.df.to_csv("HistoricalData.csv")

    @staticmethod
    def date_conversion(t, delta):
        gmtm = datetime.utcfromtimestamp(t - delta * 86400)
        yr = str(gmtm.year)
        mn = str(gmtm.month).zfill(2)
        dy = str(gmtm.day).zfill(2)
        return yr + "-" + mn + "-" + dy

    def parseData(self, json):
        self.df = pd.concat([self.df, pd.DataFrame.from_dict(json)], ignore_index=True)
