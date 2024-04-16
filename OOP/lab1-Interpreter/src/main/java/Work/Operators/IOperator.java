package Work.Operators;

import Work.Source.Context;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

public interface IOperator {
    void operation(@NotNull Context entity) throws IOException;
}
