package Calc;

import java.util.Map;
import java.util.Stack;

public class ExecutionContext {
    private Stack<Double> stack;
    private Map<String, Double> parameterMap;

    public ExecutionContext(Stack<Double> stack, Map<String, Double> parameterMap) {
        this.stack = stack;
        this.parameterMap = parameterMap;
    }

    public Stack<Double> getStack() {
        return stack;
    }

    public Map<String, Double> getParameterMap() {
        return parameterMap;
    }
}
