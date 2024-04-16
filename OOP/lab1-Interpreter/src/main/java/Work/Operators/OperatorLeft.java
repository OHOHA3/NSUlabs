package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

public class OperatorLeft implements IOperator {
    public void operation(@NotNull Context entity) {
        entity.leftPtr();
    }
}
