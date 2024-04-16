package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

public class OperatorRead implements IOperator {
    public void operation(@NotNull Context entity) throws IOException {
        byte b = (byte) System.in.read();
        entity.setByte(b);
    }
}
