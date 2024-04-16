package Work.Source;

import Work.Utilities.ExecOrConfigException;
import Work.Utilities.ParsInput;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jetbrains.annotations.NotNull;

public class SymbolAndOperator {
    private static final Logger logger = LogManager.getLogger(ParsInput.class);

    @NotNull
    private final Character symbol;
    @NotNull
    private final String operator;

    public SymbolAndOperator(String[] words) throws ExecOrConfigException {
        if (words.length != 2) {
            logger.error("invalid config: invalid format");
            throw new ExecOrConfigException("invalid config: invalid format");
        }
        this.symbol = words[0].charAt(0);
        this.operator = words[1];
    }

    public Character getSymbol() {
        return symbol;
    }

    public String getOperator() {
        return operator;
    }
}
