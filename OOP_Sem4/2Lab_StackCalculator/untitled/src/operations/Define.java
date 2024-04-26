package operations;

import Calc.ExecutionContext;
import Except.CalcExceptions;

import java.util.Map;
import java.util.Stack;

public class Define extends Oper {
    public double doOper(Object[] args) throws CalcExceptions {
        if (args.length != 3) {
            throw new CalcExceptions("Error: invalid count of args");
        }
//        Stack<Double> stack = (Stack<Double>)args[0];
        ExecutionContext ec = (ExecutionContext)args[0];
        Map<String, Double> map = ec.getParameterMap();
        String key = (String)args[1];
        double val = (double)args[2];
        map.put(key, val);
        return 0;
    }
}
