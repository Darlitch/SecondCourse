public class Node {
    public boolean isOper;
    public int val;
    public Node left, right;

    public Node(boolean isOper, int value, Node left, Node right) {
        this.isOper = isOper;
        this.val = value;
        this.left = left;
        this.right = right;
    }
}
