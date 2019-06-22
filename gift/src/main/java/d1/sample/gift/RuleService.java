package d1.sample.gift;

import jdk.nashorn.api.scripting.JSObject;
import org.apache.commons.io.FileUtils;

import javax.script.Invocable;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;
import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Random;

/**
 * @author liuyi
 */
public class RuleService {
    private static RuleService instance;
    private static SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private PointsService pointsService;
    private ScriptEngine engine;

    private RuleService() {
        pointsService = new PointsService();
        ScriptEngineManager manager = new ScriptEngineManager();
        engine = manager.getEngineByName("nashorn");
    }

    public static RuleService Instance() {
        if (instance == null) {
            instance = new RuleService();
        }
        return instance;
    }

    public void loadRule() throws IOException, ScriptException {
        String rules = FileUtils.readFileToString(new File("./config/rules.js"));
        engine.eval(rules);
    }

    /**
     * 赢取积分
     *
     * @param rule 积分规则名字
     * @param user 用户名
     * @param date 时间
     * @throws ScriptException
     * @throws NoSuchMethodException
     */
    public void earnPoint(String rule, String user, Date date) throws ScriptException, NoSuchMethodException {
        Invocable invocable = (Invocable) engine;
        JSObject result = (JSObject) invocable.invokeFunction(rule, dateFormat.format(date), user);
        //积分为0，只写日志不写数据库
        Long point = ((Integer) result.getMember("point")).longValue();
        String desc = (String) result.getMember("desc");
        System.out.println("日志:" + desc);
        if (point != 0) {
            pointsService.addRulePoint(rule, user, point, date, desc);
        }
    }

    public boolean betweenDate(String currentDate, String lowDate, String highDate) {
        //这里随便返回随机
        return new Random().nextBoolean();
    }

    public String getRegistYear(String user) {
        //应该从数据库查，这里随便返回随机
        return new Random().nextBoolean() ? "2017" : "2018";
    }

    public int getTodayTotalPoint(String user) {
        //应该从数据库查，这里随便返回随机
        return (int) (Math.random() * 21 + 1);
    }

    public void printResult() {
        pointsService.printRecord();
    }
}
