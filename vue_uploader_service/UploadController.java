package d1.project.vrservice.controller;

import d1.framework.webapi.http.Result;
import d1.framework.webapi.http.ResultUtil;
import d1.project.vrservice.model.Chunk;
import io.swagger.annotations.Api;
import org.apache.commons.io.FileUtils;
import org.apache.tomcat.util.http.fileupload.servlet.ServletFileUpload;
import org.hibernate.service.spi.ServiceException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

@RestController
@Api(value = "/upload", description = "文件上传，支持文件分片上传")
@RequestMapping("/upload")
public class UploadController {

    @Value("${d1.upload.file.path}")
    private String filePath;

    @Value("${d1.upload.file.path.temp}")
    private String filePathTemp;

    /**
     * 分片上传
     */
    @RequestMapping(method = RequestMethod.POST)
    @ResponseBody
    public Result upload(HttpServletRequest request, Chunk chunk) throws IOException {

        boolean isMultipart = ServletFileUpload.isMultipartContent(request);
        if (isMultipart) {
            MultipartFile file = chunk.getFile();

            if (file == null) {
                throw new ServiceException("参数验证失败！");
            }

            Integer chunkNumber = chunk.getChunkNumber();
            if (chunkNumber == null) {
                chunkNumber = 0;
            }

            File outFile = new File(filePathTemp + File.separator + chunk.getIdentifier(), chunkNumber + ".part");

            InputStream inputStream = file.getInputStream();
            FileUtils.copyInputStreamToFile(inputStream, outFile);
        }

        return ResultUtil.success();
    }

    /**
     * 合并所有分片
     */
    @GetMapping("/merge")
    @ResponseBody
    public Result mergeFile(String filename, String guid) throws Exception {

        File file = new File(filePathTemp + File.separator + guid);
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            if (files != null && files.length > 0) {
                File partFile = new File(filePath + File.separator + filename);
                for (int i = 1; i <= files.length; i++) {
                    File s = new File(filePathTemp + File.separator + guid, i + ".part");
                    FileOutputStream destTempfos = new FileOutputStream(partFile, true);
                    FileUtils.copyFile(s, destTempfos);
                    destTempfos.close();
                }
                FileUtils.deleteDirectory(file);
            }
        }

        return ResultUtil.success();
    }
}