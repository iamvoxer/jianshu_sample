package d1.project.yjh;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.sql.*;

@SpringBootApplication
public class YjhApplication {

    public static void main(String[] args) {

        try {
            OracleConnector.connect();
//            HybridDBConnector.connect();
        } catch (Exception e) {
            e.printStackTrace();
        }
        //SpringApplication.run(YjhApplication.class, args);
    }

}
