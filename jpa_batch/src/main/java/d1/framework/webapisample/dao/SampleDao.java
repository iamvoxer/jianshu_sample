package d1.framework.webapisample.dao;

import d1.framework.webapisample.entity.SampleEntity;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * @author Buter
 */
public interface SampleDao extends JpaRepository<SampleEntity, String>, JpaSpecificationExecutor<SampleEntity> {

}