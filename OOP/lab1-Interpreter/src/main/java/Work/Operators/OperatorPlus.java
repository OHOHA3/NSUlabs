package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

public class OperatorPlus implements IOperator {
    public void operation(@NotNull Context entity) {
        byte b = entity.getByte();
        b++;
        entity.setByte(b);
    }
}
