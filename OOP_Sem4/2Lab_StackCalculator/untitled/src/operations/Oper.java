package operations;

import Except.CalcExceptions;

public abstract class Oper {
    public abstract double Calculate(double[] argv) throws CalcExceptions;
}

