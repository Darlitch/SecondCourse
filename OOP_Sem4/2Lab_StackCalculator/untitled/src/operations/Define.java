package operations;

import Calc.ExecutionContext;
import Except.CalcExceptions;

import java.util.Map;

public class Define extends Oper {
//    private Define(){
//        super();
//    }
//    public static Define newInstance() {
//        return new Define();
//    }
    public void doOper(Object[] args) throws CalcExceptions {
        if (args.length != 3) {
            throw new CalcExceptions("Error: invalid count of args");
        }
//        Stack<Double> stack = (Stack<Double>)args[0];
        ExecutionContext ec = (ExecutionContext)args[0];
        Map<String, Double> map = ec.getParameterMap();
        String key = (String)args[1];
        double val = (double)args[2];
        map.put(key, val);
    }
}
