package core.trade;

public interface ITrade {
    /**
     * Sell at market price
     * @param quantity
     * @return t - success f - fail
     */
    public boolean placeSellOrder(double quantity);

    /**
     * Buy at market price
     * @param quantity
     * @return t - success f - fail
     */
    public boolean placeBuyOrder(double quantity);

    /**
     * place bid
     * @param price bid price
     * @param quantity
     * @return t - success f - fail
     */
    public boolean placeBid(double price, double quantity);

    /**
     *
     * @param price asking price
     * @param quantity
     * @return t - success f - fail
     */
    public boolean placeAsk(double price, double quantity);
}
