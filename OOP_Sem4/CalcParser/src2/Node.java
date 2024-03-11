public class Node {
    public boolean isOper;

    public int sym;
    public Result right = null;
    public Result left = null;

    public Result(boolean isOper, int sym, char sym, Result left, Result right) {
        this.right = right;
        this.left = left;
    }
}
