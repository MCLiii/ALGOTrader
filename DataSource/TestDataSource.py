from TiingoApi import TiingoApi
from datetime import datetime


def test():
    T = TiingoApi("api key here")
    T.set_history_len(300)
    T.set_sample_freq(10)
    T.set_ticker_symbol("btcusd", ["/tiingo/crypto/"])
    T.get_history_data(int(datetime.utcnow().timestamp()))
