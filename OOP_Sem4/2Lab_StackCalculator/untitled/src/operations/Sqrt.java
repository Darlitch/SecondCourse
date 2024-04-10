package operations;

import Except.CalcExceptions;

public class Sqrt extends Oper {
    public double Calculate(double[] argv) throws CalcExceptions {
        if (argv.length != 1) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        return Math.sqrt(argv[0]);
    }
}
