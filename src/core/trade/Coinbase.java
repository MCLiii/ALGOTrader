package core.trade;

public class Coinbase implements ITrade{
    @Override
    public boolean placeSellOrder(double quantity) {
        return false;
    }

    @Override
    public boolean placeBuyOrder(double quantity) {
        return false;
    }

    @Override
    public boolean placeBid(double price, double quantity) {
        return false;
    }

    @Override
    public boolean placeAsk(double price, double quantity) {
        return false;
    }
}
