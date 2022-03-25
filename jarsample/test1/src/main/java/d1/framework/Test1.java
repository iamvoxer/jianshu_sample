package d1.framework;

import org.springframework.stereotype.Service;

@Service("TEST1_SERVICE")
public class Test1 implements iTest{
    @Override
    public String testfunction() {
        return "tes1......................";
    }
}
