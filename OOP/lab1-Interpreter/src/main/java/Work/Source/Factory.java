package Work.Source;

import Work.Main;
import Work.Operators.IOperator;
import Work.Utilities.ExecOrConfigException;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Factory {
    private static final Logger logger = LogManager.getLogger(Main.class);
    private final Map<Character, String> symbolMap = new HashMap<>();

    public Factory(@NotNull ArrayList<SymbolAndOperator> operators) throws ExecOrConfigException {
        for (SymbolAndOperator sao : operators) {
            registerCreator(sao.getSymbol(), sao.getOperator());
        }
    }

    void registerCreator(@NotNull Character operatorSymbol, @NotNull String operator) throws ExecOrConfigException {
        if (symbolMap.containsKey(operatorSymbol)) {
            logger.error("invalid config: using similar symbols");
            throw new ExecOrConfigException("invalid config: using similar symbols");
        }
        if (symbolMap.containsValue(operator)) {
            logger.error("invalid config: using similar Classes");
            throw new ExecOrConfigException("invalid config: using similar Classes");
        }
        symbolMap.put(operatorSymbol, operator);
    }

    public @NotNull IOperator create(@NotNull Character operatorSymbol) throws ExecOrConfigException {
        if (!symbolMap.containsKey(operatorSymbol)) {
            logger.error("invalid Executable file: uncorrected symbols");
            throw new ExecOrConfigException("invalid Executable file: uncorrected symbols");
        }
        IOperator unit;
        try {
            String nameOfClass = symbolMap.get(operatorSymbol);
            unit = (IOperator) Class.forName(nameOfClass).getClassLoader().loadClass(nameOfClass).newInstance();
        } catch (InstantiationException | IllegalAccessException | ClassNotFoundException e) {
            logger.error("invalid config: this class doesn't exist");
            throw new ExecOrConfigException("invalid config: this class doesn't exist");
        }
        return unit;
    }
}
