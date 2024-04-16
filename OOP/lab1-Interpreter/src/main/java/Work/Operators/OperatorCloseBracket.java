package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

public class OperatorCloseBracket implements IOperator {
    public void operation(@NotNull Context entity) {
        if (entity.getByte() != 0) {
            int bracket = 1;
            while (bracket != 0) {
                entity.leftCodePtr();
                if (entity.getOperation() == entity.getSymbolOfCloseBracket()) {
                    bracket++;
                }
                if (entity.getOperation() == entity.getSymbolOfOpenBracket()) {
                    bracket--;
                }
            }
        }
    }
}
