public class Main {
    public static void main(String[] args) {
        String formula = "2+2*5+20";
        Parser parser = new Parser(formula);
        Node tree = parser.S();
//        TreeCalc calc = new TreeCalc();
        System.out.println(TreeCalc.Calc(tree));
    }
}