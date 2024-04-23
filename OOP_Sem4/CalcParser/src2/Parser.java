public class Parser {
    private final String str;
    private int pointer = 0;
    Parser(String str) {
        this.str = str;
    }

    Node S() { // цикл!
        Node leftNode = M();
        if (pointer == str.length()) {
            return leftNode;
        }
        while (str.charAt(pointer) == '+' || str.charAt(pointer) == '-') {
            int val = str.charAt(pointer);
            pointer++;
            Node rightNode = M();
            leftNode = new Node(true,val, leftNode, rightNode);
            if (pointer == str.length()) {
                return leftNode;
            }
        }
        return leftNode;
    }

    Node M() {
        Node leftNode = P();
        if (pointer == str.length()) {
            return leftNode;
        }
        while (str.charAt(pointer) == '*' || str.charAt(pointer) == '/') {
            int val = str.charAt(pointer);
            pointer++;
            Node rightNode = P();
            leftNode = new Node(true,val, leftNode, rightNode);
            if (pointer == str.length()) {
                return leftNode;
            }
        }
        return leftNode;
    }

    Node P() {
        if (pointer == str.length()) {
            return null;
        }
        if (str.charAt(pointer) == '(') {
            pointer++;
            Node node = S();
            if (str.charAt(pointer) == ')') {
                pointer++;
                return node;
            }
        }
        if ((int)str.charAt(pointer) <= (int)'9' && (int)str.charAt(pointer) >= (int)'0') {
            int start = pointer;
            while(pointer != str.length() && str.charAt(pointer) <= '9' && str.charAt(pointer) >= '0') {
                pointer++;
            }
            return new Node(false, Integer.parseInt(str.substring(start, pointer)), null, null);
        }
        return null;
    }
}