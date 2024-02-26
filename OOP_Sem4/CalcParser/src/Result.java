public class Result {
    public double res;
    public String rest;
    public Result right = null;
    public Result left = null;

    public Result(double a, String r, Result left, Result right) {
        this.res = a;
        this.rest = r;
        this.right = right;
        this.left = left;
    }
}
