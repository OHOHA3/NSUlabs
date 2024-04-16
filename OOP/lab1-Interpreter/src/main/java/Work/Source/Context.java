package Work.Source;

import Work.Operators.OperatorCloseBracket;
import Work.Operators.OperatorOpenBracket;
import Work.Utilities.ExecOrConfigException;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public class Context {
    private char symbolOfOpenBracket;
    private char symbolOfCloseBracket;
    private final byte[] array = new byte[30000];
    private int ptr = 0;
    private final ArrayList<Character> code;
    private int codePtr = 0;

    public Context(@NotNull ArrayList<Character> code, @NotNull ArrayList<SymbolAndOperator> operators) throws ExecOrConfigException {
        this.code = code;
        for (SymbolAndOperator sao : operators) {
            if (sao.getOperator().equals(OperatorOpenBracket.class.getName())) {
                symbolOfOpenBracket = sao.getSymbol();
            }
            if (sao.getOperator().equals(OperatorCloseBracket.class.getName())) {
                symbolOfCloseBracket = sao.getSymbol();
            }
        }
    }

    public void rightPtr() {
        ptr++;
    }

    public void leftPtr() {
        ptr--;
    }

    public byte getByte() {
        return array[ptr];
    }

    public void setByte(byte b) {
        array[ptr] = b;
    }

    public char getOperation() {
        return code.get(codePtr);
    }

    public void leftCodePtr() {
        codePtr--;
    }

    public void rightCodePtr() {
        codePtr++;
    }

    public char getSymbolOfOpenBracket() {
        return symbolOfOpenBracket;
    }

    public char getSymbolOfCloseBracket() {
        return symbolOfCloseBracket;
    }

    boolean haveNext() {
        return codePtr < code.size();
    }
}
