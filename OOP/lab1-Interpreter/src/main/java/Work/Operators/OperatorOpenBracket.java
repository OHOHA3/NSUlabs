package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

public class OperatorOpenBracket implements IOperator {
    public void operation(@NotNull Context entity) {
        if (entity.getByte() == 0) {
            int bracket = 1;
            while (bracket != 0) {
                entity.rightCodePtr();
                if (entity.getOperation() == entity.getSymbolOfOpenBracket()) {
                    bracket++;
                }
                if (entity.getOperation() == entity.getSymbolOfCloseBracket()) {
                    bracket--;
                }
            }
        }
    }
}
