package operations;

import Except.CalcExceptions;

import java.util.Stack;

public class Print extends Oper {
    public double doOper(Object[] args) throws CalcExceptions {
        if (args.length != 1) {
            throw new CalcExceptions("Error: invalid count of args");
        }
        Stack<Double> stack = (Stack<Double>)args[0];
        return stack.peek();
    }
}
