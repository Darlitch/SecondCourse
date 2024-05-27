package controller;

public class Ticker extends Thread {
    private TickerListener tickerListener;

    public void setTickerListener(TickerListener tickerListener) {
        this.tickerListener = tickerListener;
    }

    @Override
    public void run() {
        while (!isInterrupted()) {
            try {
                Thread.sleep(15);
                if (tickerListener != null)
                    tickerListener.ticked();
            } catch (InterruptedException e) {
                break;
            }
        }
    }
}