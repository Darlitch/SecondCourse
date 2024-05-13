package operations;

import Except.CalcExceptions;

public abstract class Oper {
    protected Oper(){}
    public abstract void doOper(Object[] args) throws CalcExceptions;

    public static Oper newInstance() {
        return null;
    }
}

