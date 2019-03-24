package d1.project.randomer;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.sql.SQLException;
import java.util.Date;

@RestController
@RequestMapping("/random")
public class RandomerController {
    private final BuildService buildService;

    @Autowired
    public RandomerController(BuildService buildService) {
        this.buildService = buildService;
    }

    @RequestMapping(value = "/build", method = RequestMethod.GET)
    public String build() {
        long start = new Date().getTime();
        try {
            buildService.run();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            return "执行失败：" + e.getMessage();
        }
        long end = new Date().getTime();
        long during = end - start;
        return "执行结束,耗时" + during + "毫秒";
    }
}
