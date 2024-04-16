package Work.Source;

import Work.Operators.IOperator;
import Work.Utilities.ExecOrConfigException;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.ArrayList;

public class Interpreter {
    public void interpretation(@NotNull ArrayList<Character> code, @NotNull ArrayList<SymbolAndOperator> operators) throws ExecOrConfigException, IOException {
        Context entity = new Context(code, operators);
        Factory factory = new Factory(operators);
        while (entity.haveNext()) {
            IOperator operator = factory.create(entity.getOperation());
            operator.operation(entity);
            entity.rightCodePtr();
        }
    }
}
