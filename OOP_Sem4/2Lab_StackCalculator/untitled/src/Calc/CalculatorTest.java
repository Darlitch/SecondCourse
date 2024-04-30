package Calc;

import Except.CalcExceptions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import static org.junit.jupiter.api.Assertions.*;

public class CalculatorTest {
    @Test
    void DefineTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 5");
        commands.add("DeFiNe boba 2");
        parser.parsing(commands);
        Map<String, Double> map = parser.ec.getParameterMap();
        assertEquals(5.0, map.get("a"));
        assertEquals(2.0, map.get("boba"));
    }

    @Test
    void PushTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 5");
        commands.add("DeFiNe boba 2");
        commands.add("push a");
        commands.add("PUSH boba");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertEquals(2.0, stack.pop());
        assertEquals(5.0, stack.pop());
    }

    @Test
    void PopTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 5");
        commands.add("DeFiNe boba 2");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("POP");
        commands.add("pop");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertTrue(stack.isEmpty());
    }

    @Test
    void AddTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 8");
        commands.add("DeFiNe boba 2");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("+");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertEquals(10.0, stack.pop());
        commands.clear();
        commands.add("define a 24.4");
        commands.add("DeFiNe boba 12.3");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("+");
        parser.parsing(commands);
        assertEquals(36.7, stack.pop());
    }

    @Test
    void MultTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 5");
        commands.add("DeFiNe boba 3");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("*");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertEquals(15.0, stack.pop());
        commands.clear();
        commands.add("define a 24.12");
        commands.add("DeFiNe boba 12.24");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("*");
        parser.parsing(commands);
        assertTrue(Math.abs(295.2288 - stack.pop()) < 0.00001);
    }

    @Test
    void SubTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 5");
        commands.add("DeFiNe boba 3");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("-");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertEquals(2.0, stack.pop());
        commands.clear();
        commands.add("define a 24.12");
        commands.add("DeFiNe boba 12.24");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("-");
        parser.parsing(commands);
        assertEquals(11.88, stack.pop());
    }

    @Test
    void DivTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 288");
        commands.add("DeFiNe boba 12");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("/");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertEquals(24.0, stack.pop());
        commands.clear();
        commands.add("define a 357.0204");
        commands.add("DeFiNe boba 12.58");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("/");
        parser.parsing(commands);
        assertTrue(Math.abs(28.38 - stack.pop()) < 0.0001);
    }

    @Test
    void SqrtTest() throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 8");
        commands.add("DeFiNe boba 2");
        commands.add("push a");
        commands.add("PUSH boba");
        commands.add("*");
        commands.add("sqrt");
        parser.parsing(commands);
        Stack<Double> stack = parser.ec.getStack();
        assertEquals(4.0, stack.pop());
        commands.clear();
        commands.add("define a 64");
        commands.add("push a");
        commands.add("sqrt");
        parser.parsing(commands);
        assertEquals(8.0, stack.pop());
    }

    @Test
    void AddExceptionsTest () throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 8");
        commands.add("push a");
        commands.add("+");
        try {
            parser.parsing(commands);
            fail("Error: There are not enough elements in the stack to perform the operation");
        } catch (final CalcExceptions e) {
            assertTrue(true);
        }
    }

    @Test
    void SubExceptionsTest () throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 8");
        commands.add("push a");
        commands.add("-");
        try {
            parser.parsing(commands);
            fail("Error: There are not enough elements in the stack to perform the operation");
        } catch (final CalcExceptions e) {
            assertTrue(true);
        }
    }

    @Test
    void MultExceptionsTest () throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 8");
        commands.add("push a");
        commands.add("*");
        try {
            parser.parsing(commands);
            fail("Error: There are not enough elements in the stack to perform the operation");
        } catch (final CalcExceptions e) {
            assertTrue(true);
        }
    }

    @Test
    void DivExceptionsTest () throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a 8");
        commands.add("push a");
        commands.add("-");
        try {
            parser.parsing(commands);
            fail("Error: There are not enough elements in the stack to perform the operation");
        } catch (final CalcExceptions e) {
            assertTrue(true);
        }
    }

    @Test
    void CVExceptionsTest () throws CalcExceptions {
        Parser parser = new Calculator();
        List<String> commands = new ArrayList<>();
        commands.add("define a");
        try {
            parser.parsing(commands);
            fail("Error: Invalid number of command arguments: DEFINE");
        } catch (final CalcExceptions e) {
            assertTrue(true);
        }
        commands.clear();
        commands.add("aboba a");
        try {
            parser.parsing(commands);
            fail("Error: Invalid number of command arguments: ABOBA");
        } catch (final CalcExceptions e) {
            assertTrue(true);
        }
    }
}