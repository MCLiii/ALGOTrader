import abc


class DataSource:
    @abc.abstractmethod
    def set_ticker_symbol(self, symbol, params):
        raise NotImplementedError

    @abc.abstractmethod
    def set_history_len(self, days):
        raise NotImplementedError

    @abc.abstractmethod
    def get_live_price(self):
        raise NotImplementedError

    @abc.abstractmethod
    def data_updated(self, data):
        # returns a boolean to indicate weather data has been updated
        raise NotImplementedError

    @abc.abstractmethod
    def start_live_update(self):
        raise NotImplementedError

    @abc.abstractmethod
    def set_sample_freq(self, min):
        raise NotImplementedError
