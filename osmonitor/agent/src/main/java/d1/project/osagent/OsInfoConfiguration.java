package d1.project.osagent;

import d1.framework.osinfo.service.OsInfoService;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.io.IOException;

/**
 * @author Kikki
 */
@Configuration
public class OsInfoConfiguration {

    @Bean
    OsInfoService osInfo() throws IOException {
        return new OsInfoService();
    }
}
