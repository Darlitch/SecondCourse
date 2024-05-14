package Calc;

import Except.CalcExceptions;
import operations.*;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.function.Supplier;

public class Calculator extends Parser {
    private static final Map<String, Supplier<Oper>> factoryMap = new HashMap<>();
    static {
        String fileName = "C:\\SecondCourse\\OOP_Sem4\\2Lab_StackCalculator\\untitled\\src\\configs\\cfg.txt";
        try (Scanner scanner = new Scanner(new File(fileName))) {
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] words = line.split("\\s+");
                Class<?> clazz = Class.forName("operations." + words[1]);
                Constructor<?> constructor = clazz.getConstructor();
                Supplier<Oper> instanceSupplier = () -> {
                    try {
                        return (Oper) constructor.newInstance();
                    } catch (InstantiationException | IllegalAccessException | InvocationTargetException e) {
                        throw new RuntimeException("Failed to create instance", e);
                    }
                };
                factoryMap.put(words[0], instanceSupplier);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        } catch (NoSuchMethodException e) {
            throw new RuntimeException(e);
        }
//        factoryMap.put("DEFINE", Define::newInstance);
//        factoryMap.put("PUSH", Push::newInstance);
//        factoryMap.put("POP", Pop::newInstance);
//        factoryMap.put("PRINT", Print::newInstance);
//        factoryMap.put("SQRT", Sqrt::newInstance);
//        factoryMap.put("+", Add::newInstance);
//        factoryMap.put("-", Sub::newInstance);
//        factoryMap.put("*", Mult::newInstance);
//        factoryMap.put("/", Div::newInstance);
    }
    protected Oper newInstance(String cmd) throws CalcExceptions {
//        Oper oper = null;
//        oper = switch (cmd) {
//            case "DEFINE" -> new Define();
//            case "PUSH" -> new Push();
//            case "POP" -> new Pop();
//            case "PRINT" -> new Print();
//            case "SQRT" -> new Sqrt();
//            case "+" -> new Add();
//            case "-" -> new Sub();
//            case "*" -> new Mult();
//            case "/" -> new Div();
//            default -> throw new CalcExceptions("Error CalcOper: Unknown command: " + oper);
//        };
//        return oper;
        Supplier<Oper> operSupplier = factoryMap.get(cmd);
        if (operSupplier != null) {
            return operSupplier.get();
        } else {
            throw new IllegalArgumentException("Error: Unknown command: " + cmd);
        }
    }

}

