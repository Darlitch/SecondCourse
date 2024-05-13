package operations;

import Calc.ExecutionContext;
import Except.CalcExceptions;

import java.util.Map;
import java.util.Stack;

public class Push extends Oper {
    private Push(){
        super();
    }
    public static Push newInstance() {
        return new Push();
    }
    public void doOper(Object[] args) throws CalcExceptions {
//        if (args.length != 2) {
//            throw new CalcExceptions("Error: invalid count of args");
//        }
//        Stack<Double> stack = (Stack<Double>)args[0];
        if (args.length != 2) {
            throw new CalcExceptions("Error: invalid count of argv");
        }
        ExecutionContext ec = (ExecutionContext)args[0];
        Stack<Double> stack = ec.getStack();
        Map<String,Double> map = ec.getParameterMap();
        double val = map.get((String)args[1]);
        stack.push(val);
    }
}
