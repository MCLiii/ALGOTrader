package core.data;

import java.util.List;

public class CoinBaseData implements IData{

    @Override
    public double setTickerSymbol(String symbol) {
        return 0;
    }

    @Override
    public List<Record> getHistory(int startTime, int endTime, int sampleWidth) {
        return null;
    }

    @Override
    public double getCurrentPrice() {
        return 0;
    }
}
