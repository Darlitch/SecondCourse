package Calc;

import Except.CalcExceptions;
import operations.Oper;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Stack;

public abstract class Parser {
    ExecutionContext ec;
    public Parser() {
        Stack<Double> stack = new Stack<>();
        Map<String, Double> map = new HashMap<>();
        ec = new ExecutionContext(stack, map);
    }

    public void parsing(List<String> commands) throws CalcExceptions {
        for(String cmd: commands) {
            String[] args = cmd.split("\\s+");
            if (args.length == 0) {
                continue;
            }
            String upperCaseCmd = args[0].toUpperCase();
            CommandValidator.validateCommand(upperCaseCmd, args.length);
            Object[] argsObj = takeObjArgs(args, CommandValidator.CmdLength(upperCaseCmd));
            Oper oper = calculating(upperCaseCmd);
            oper.doOper(argsObj);
        }
//        Oper oper = calculating("PRINT");
//        oper.doOper(new Object[]{ec});
    }

    private Object[] takeObjArgs(String[] args, int lenArgs) {
        Object[] argsObj = new Object[lenArgs];
        argsObj[0] = ec;
        switch (lenArgs) {
            case 3:
                argsObj[1] = args[1];
                argsObj[2] = Double.parseDouble(args[2]);
                break;
            case 2:
                argsObj[1] = args[1];
                break;
            default:
                break;
        }
        return argsObj;
    }

    protected abstract Oper calculating(String oper) throws CalcExceptions;
}
