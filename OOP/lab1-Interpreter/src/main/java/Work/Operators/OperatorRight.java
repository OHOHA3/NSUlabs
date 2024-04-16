package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

public class OperatorRight implements IOperator {
    public void operation(@NotNull Context entity) {
        entity.rightPtr();
    }
}
