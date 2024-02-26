public class Main {
    public static void main(String[] args) {
        String formula = "(2-2)*2";
        MathParser mp = new MathParser();
        try {
            System.out.println("= " + mp.Parse(formula));
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }
}