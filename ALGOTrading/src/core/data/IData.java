package core.data;

import java.util.List;

public interface IData {
    public List<Record> getHistory(int startTime, int endTime, int sampleWidth);

    public double getCurrentPrice();
}
