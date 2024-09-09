public class TreeCalc {
    public static int Calc(Node node) {
        int res = 0;
        if (node == null) {
            return 0;
        }
        if (node.isOper) {
            if (node.val == '+') {
                res = Calc(node.left) + Calc(node.right);
            }
            if (node.val == '-') {
                res = Calc(node.left) - Calc(node.right);
            }
            if (node.val == '*') {
                res = Calc(node.left) * Calc(node.right);
            }
            if (node.val == '/') {
                res = Calc(node.left) / Calc(node.right);
            }
        } else {
            res = node.val;
        }
        return res;
    }
}
