package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

public class OperatorWrite implements IOperator {
    public void operation(@NotNull Context entity) {
        System.out.print((char) entity.getByte());
    }
}
