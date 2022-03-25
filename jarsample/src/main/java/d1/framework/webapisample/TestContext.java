package d1.framework.webapisample;

import d1.framework.iTest;
import org.springframework.stereotype.Service;

import java.util.Map;

@Service
public class TestContext {

    private final Map<String, iTest> iTestMap;

    public TestContext(Map<String, iTest> iTestMap) {
        this.iTestMap = iTestMap;
    }

    public iTest getiTest(String name) {
        return iTestMap.get(name);
    }
}
