-- MySQL Script generated by MySQL Workbench
-- Thursday, May 09, 2019 AM11:18:00 CST
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='';

-- -----------------------------------------------------
-- Schema rojcpp
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `rojcpp` ;

-- -----------------------------------------------------
-- Schema rojcpp
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `rojcpp` DEFAULT CHARACTER SET utf8mb4 ;
SHOW WARNINGS;
USE `rojcpp` ;

-- -----------------------------------------------------
-- Table `rojcpp`.`users`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`users` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`users` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `username` VARCHAR(64) NOT NULL DEFAULT '',
  `nickname` VARCHAR(64) NOT NULL DEFAULT '',
  `password` VARCHAR(255) NOT NULL DEFAULT '',
  `school` VARCHAR(32) NOT NULL DEFAULT '',
  `email` VARCHAR(255) NOT NULL DEFAULT '',
  `role` VARCHAR(64) NOT NULL DEFAULT 'student' COMMENT '\'student\',\'teacher\',\'admin\'',
  `email_verified_at` TIMESTAMP NOT NULL DEFAULT 0,
  `status` TINYINT NOT NULL DEFAULT 0 COMMENT '-1: lock\n0: normal\n1: need verify by admin',
  `remember_token` VARCHAR(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE INDEX `uq_email` (`deleted_at` ASC, `email` ASC),
  UNIQUE INDEX `uq_username` (`deleted_at` ASC, `username` ASC))
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`login_logs`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`login_logs` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`login_logs` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3),
  `updated_at` TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3) ON UPDATE CURRENT_TIMESTAMP(3),
  `user_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `expireat` timestamp NOT NULL DEFAULT (current_timestamp() + interval 15 day),
  `ip` VARCHAR(40) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id` ASC),
  INDEX `idx_login_time` (`created_at` ASC),
  CONSTRAINT `fk_login_user`
    FOREIGN KEY (`user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`problems`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`problems` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`problems` (
  -- `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `id` INT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `owner_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `title` VARCHAR(128) NOT NULL DEFAULT '',
  `source` VARCHAR(128) NOT NULL DEFAULT '',
  `time_limit` INT UNSIGNED NOT NULL DEFAULT 1000 COMMENT 'mirco seconds',
  `memory_limit` INT UNSIGNED NOT NULL DEFAULT 1024 COMMENT 'kbytes',
  `is_special_judge` TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT '0: not special judge\n1: special judge',
  `type` TINYINT NOT NULL DEFAULT 0 COMMENT '0: normal\n1: supplement after submit code.\n2: supplement before judge',
  `similar_from` BIGINT UNSIGNED NOT NULL DEFAULT 0,
  `status` TINYINT NOT NULL DEFAULT 0 COMMENT '0: normal\n1: hide',
  `testdata_updated_at` TIMESTAMP NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`owner_id` ASC),
  INDEX `idx_source` (`source` ASC),
  INDEX `idx_title` (`title` ASC),
  CONSTRAINT `fk_problem_owner`
    FOREIGN KEY (`owner_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`contests`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`contests` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`contests` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `owner_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `name` VARCHAR(255) NOT NULL DEFAULT '',
  `start_at` DATETIME NOT NULL DEFAULT 0,
  `end_before` DATETIME NOT NULL DEFAULT 0,
  `privilege` TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT '0: public\n1: private\n2: protect\n3: need register',
  `privilege_info` VARCHAR(384) NOT NULL DEFAULT '{}',
  `allow_language` BIGINT NOT NULL DEFAULT 0 COMMENT '0 is all allow, if a bit set 1, that mean the kind of lang not allow',
  `lock_rank_at` TINYINT NOT NULL DEFAULT 0 COMMENT '0 for no, 20 for at last 20% time',
  PRIMARY KEY (`id`),
  INDEX `idx_owner` (`owner_id` ASC),
  INDEX `idx_start` (`start_at` ASC),
  INDEX `idx_privilege` (`privilege` ASC),
  CONSTRAINT `fk_contest_user`
    FOREIGN KEY (`owner_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`options`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`options` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`options` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `key` VARCHAR(64) NOT NULL DEFAULT '',
  `value` TEXT NOT NULL DEFAULT '' COMMENT 'json',
  `comment` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE INDEX `uk_id` (`key` ASC))
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`contest_problems`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`contest_problems` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`contest_problems` (
  `id` BIGINT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `contest_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `problem_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `title` VARCHAR(128) NOT NULL DEFAULT '',
  `problem_order` TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'the serial number of problem',
  PRIMARY KEY (`id`),
  INDEX `idx_contest` (`contest_id` ASC),
  INDEX `idx_problem` (`problem_id` ASC),
  UNIQUE INDEX `uq_contest_problem` (`contest_id` ASC, `problem_id` ASC),
  CONSTRAINT `fk_contest_problem_contest`
    FOREIGN KEY (`contest_id`)
    REFERENCES `rojcpp`.`contests` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_contest_problem_problem`
    FOREIGN KEY (`problem_id`)
    REFERENCES `rojcpp`.`problems` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`contest_registers`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`contest_registers` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`contest_registers` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `contest_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `user_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `actual_name` VARCHAR(64) NOT NULL DEFAULT '',
  `college` VARCHAR(64) NOT NULL DEFAULT '',
  `discipline` VARCHAR(64) NOT NULL DEFAULT '' COMMENT 'it means: a branch of knowledge, typically one studied in higher education.',
  `sex` TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT '0: secert\n1: male\n2: female',
  `status` TINYINT NOT NULL DEFAULT -1 COMMENT '-1: pending\n0: accept\n1: wait for update info\n2: reject',
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id` ASC),
  INDEX `idx_contest` (`contest_id` ASC),
  UNIQUE INDEX `uq_user_contest_deleted` (`contest_id` ASC, `user_id` ASC, `deleted_at` ASC),
  CONSTRAINT `fk_contest_register_contest`
    FOREIGN KEY (`contest_id`)
    REFERENCES `rojcpp`.`contests` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_contest_register_user`
    FOREIGN KEY (`user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`contest_results`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`contest_results` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`contest_results` (
  `id` BIGINT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `contest_problem_id` BIGINT UNSIGNED NOT NULL DEFAULT 0,
  `user_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `contest_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `submit_count` INT NOT NULL DEFAULT 0 COMMENT 'value mean submit times',
  `ac_at` TIMESTAMP NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  INDEX `idx_contest_problem` (`contest_problem_id` ASC),
  INDEX `idx_user` (`user_id` ASC),
  INDEX `idx_contest` (`contest_id` ASC),
  UNIQUE INDEX `uq_user_contest_contest_problem` (`contest_problem_id` ASC, `user_id` ASC, `contest_id` ASC),
  CONSTRAINT `fk_contest_result_contest_problem`
    FOREIGN KEY (`contest_problem_id`)
    REFERENCES `rojcpp`.`contest_problems` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_contest_result_user`
    FOREIGN KEY (`user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_contest_result_contest`
    FOREIGN KEY (`contest_id`)
    REFERENCES `rojcpp`.`contests` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`mails`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`mails` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`mails` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `from_user_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `to_user_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `topic` VARCHAR(255) NOT NULL DEFAULT '',
  `status` TINYINT NOT NULL DEFAULT 0 COMMENT '0 :new mail\n1 :has read',
  PRIMARY KEY (`id`),
  INDEX `idx_to_user` (`to_user_id` ASC),
  INDEX `idx_from_user` (`from_user_id` ASC),
  INDEX `idx_time` (`created_at` DESC),
  INDEX `idx_status` (`status` ASC),
  CONSTRAINT `fk_mail_from_user`
    FOREIGN KEY (`from_user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_mail_to_user`
    FOREIGN KEY (`to_user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`solutions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`solutions` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`solutions` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `owner_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `problem_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `contest_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `hash` VARCHAR(255) NOT NULL DEFAULT '',
  `lang` TINYINT UNSIGNED NOT NULL DEFAULT 0,
  `result` TINYINT NOT NULL DEFAULT -2 COMMENT '-3:RJ WAIT\n-2:WAIT\n-1:WA\n0:AC\n1:CTLE\n2:RTLE\n3:MLE\n4:RE\n5:SE',
  `time_used` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'ms',
  `memory_used` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'kb',
  `similar_at` BIGINT UNSIGNED NOT NULL DEFAULT 0,
  `similar_percent` TINYINT UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`owner_id` ASC),
  INDEX `idx_problem` (`problem_id` ASC),
  INDEX `idx_result` (`result` ASC),
  INDEX `idx_contest_id` (`contest_id` ASC),
  CONSTRAINT `fk_solution_user`
    FOREIGN KEY (`owner_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_solution_problem`
    FOREIGN KEY (`problem_id`)
    REFERENCES `rojcpp`.`problems` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`discuss_topics`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`discuss_topics` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`discuss_topics` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `owner_id` INT UNSIGNED NOT NULL,
  `contest_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `problem_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `title` VARCHAR(255) NOT NULL,
  `status` TINYINT NOT NULL DEFAULT 0 COMMENT 'r->l count, 0 = false.\n0bit: is locked\n1bit: is topping',
  `views` BIGINT UNSIGNED NOT NULL DEFAULT 0,
  `replies` INT UNSIGNED NOT NULL DEFAULT 0,
  `latest_reply_at` DATETIME NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`owner_id` ASC),
  INDEX `idx_contest` (`contest_id` ASC),
  INDEX `idx_problem` (`problem_id` ASC),
  CONSTRAINT `fk_discuss_topic_user`
    FOREIGN KEY (`owner_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`discuss_posts`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`discuss_posts` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`discuss_posts` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `owner_id` INT UNSIGNED NOT NULL,
  `topic_id` INT UNSIGNED NOT NULL,
  `status` TINYINT NOT NULL DEFAULT 0 COMMENT 'r->l count, 0 = false.\n0bit: is locked\n1bit: is topping\n2bit: is not show',
  PRIMARY KEY (`id`),
  INDEX `idx_topic_id` (`topic_id` ASC),
  INDEX `idx_owner` (`owner_id` ASC),
  CONSTRAINT `fk_discuss_post_topic`
    FOREIGN KEY (`topic_id`)
    REFERENCES `rojcpp`.`discuss_topics` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_discuss_post_owner`
    FOREIGN KEY (`owner_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`contest_privileges`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`contest_privileges` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`contest_privileges` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `contest_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `user_id` INT UNSIGNED NOT NULL DEFAULT 0,
  `type` INT NOT NULL DEFAULT 0 COMMENT '0: allow take part in\n1: allow manage',
  PRIMARY KEY (`id`),
  INDEX `idx_user` (`user_id` ASC),
  INDEX `idx_contest` (`contest_id` ASC),
  INDEX `uq_contest_user_type` (`contest_id` ASC, `user_id` ASC, `type` ASC),
  CONSTRAINT `fk_contest_privilege_user`
    FOREIGN KEY (`user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_contest_privilege_contest`
    FOREIGN KEY (`contest_id`)
    REFERENCES `rojcpp`.`contests` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`news`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`news` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`news` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `owner_id` INT UNSIGNED NOT NULL,
  `title` VARCHAR(64) NOT NULL,
  `status` TINYINT NOT NULL DEFAULT 0 COMMENT '-1:hide\n0: normal\n1: toped',
  PRIMARY KEY (`id`),
  INDEX `idx_owner` (`owner_id` ASC),
  INDEX `idx_status` (`status` ASC),
  CONSTRAINT `fk_news_owner`
    FOREIGN KEY (`owner_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`solution_codes`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`solution_codes` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`solution_codes` (
  `solution_id` BIGINT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `code` TEXT NOT NULL,
  PRIMARY KEY (`solution_id`),
  CONSTRAINT `id`
    FOREIGN KEY (`solution_id`)
    REFERENCES `rojcpp`.`solutions` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`groups`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`groups` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`groups` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deleted_at` TIMESTAMP NOT NULL DEFAULT 0,
  `name` VARCHAR(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE INDEX `uq_name` (`name` ASC, `deleted_at` ASC))
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`user_groups`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`user_groups` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`user_groups` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `user_id` INT UNSIGNED NOT NULL,
  `group_id` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `idx_user_id` (`user_id` ASC),
  INDEX `idx_group_id` (`group_id` ASC),
  CONSTRAINT `fk_user_id`
    FOREIGN KEY (`user_id`)
    REFERENCES `rojcpp`.`users` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_group_id`
    FOREIGN KEY (`group_id`)
    REFERENCES `rojcpp`.`groups` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`problem_descriptions`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`problem_descriptions` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`problem_descriptions` (
  `problem_id` INT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `description` TEXT NOT NULL DEFAULT '',
  `input` TEXT NOT NULL DEFAULT '',
  `output` TEXT NOT NULL DEFAULT '',
  `sample` TEXT NOT NULL DEFAULT '' COMMENT 'include sample input and sample output',
  `hint` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`problem_id`),
  CONSTRAINT `fk_problem_id`
    FOREIGN KEY (`problem_id`)
    REFERENCES `rojcpp`.`problems` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`solution_full_results`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`solution_full_results` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`solution_full_results` (
  `solution_id` BIGINT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `data` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`solution_id`),
  CONSTRAINT `fk_solution_ full_result_1`
    FOREIGN KEY (`solution_id`)
    REFERENCES `rojcpp`.`solutions` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`contest_ip_limits`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`contest_ip_limits` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`contest_ip_limits` (
  `contest_id` INT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `deny_ips` TEXT NOT NULL DEFAULT '' COMMENT 'JSON CIDR',
  `allow_ips` TEXT NOT NULL DEFAULT '' COMMENT 'JSON CIDR',
  PRIMARY KEY (`contest_id`),
  CONSTRAINT `fk_contest_id`
    FOREIGN KEY (`contest_id`)
    REFERENCES `rojcpp`.`contests` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`mail_contexts`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`mail_contexts` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`mail_contexts` (
  `mail_id` BIGINT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `context` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`mail_id`),
  CONSTRAINT `fk_mail_id`
    FOREIGN KEY (`mail_id`)
    REFERENCES `rojcpp`.`mails` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`news_contexts`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`news_contexts` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`news_contexts` (
  `news_id` INT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `context` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`news_id`),
  CONSTRAINT `fk_news_id`
    FOREIGN KEY (`news_id`)
    REFERENCES `rojcpp`.`news` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`discuss_post_contexts`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`discuss_post_contexts` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`discuss_post_contexts` (
  `discuss_post_id` BIGINT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `context` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`discuss_post_id`),
  CONSTRAINT `fk_discuss_post_contexts_1`
    FOREIGN KEY (`discuss_post_id`)
    REFERENCES `rojcpp`.`discuss_posts` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

-- -----------------------------------------------------
-- Table `rojcpp`.`problem_extra_codes`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `rojcpp`.`problem_extra_codes` ;

SHOW WARNINGS;
CREATE TABLE IF NOT EXISTS `rojcpp`.`problem_extra_codes` (
  `problem_id` INT UNSIGNED NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `blank_count` TINYINT UNSIGNED NOT NULL DEFAULT 0,
  `extra_code` TEXT NOT NULL DEFAULT '',
  PRIMARY KEY (`problem_id`),
  CONSTRAINT `fk_problem_supplement_codes_1`
    FOREIGN KEY (`problem_id`)
    REFERENCES `rojcpp`.`problems` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

SHOW WARNINGS;

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `rojcpp`.`options`
-- -----------------------------------------------------
START TRANSACTION;
USE `rojcpp`;
INSERT INTO `rojcpp`.`options` (`id`, `created_at`, `updated_at`, `key`, `value`, `comment`) VALUES (1, DEFAULT, DEFAULT, 'system_status', '[0]', '0: normal, 1: in vip contest');
INSERT INTO `rojcpp`.`options` (`id`, `created_at`, `updated_at`, `key`, `value`, `comment`) VALUES (2, DEFAULT, DEFAULT, 'deny_login_ips', '{}', 'the ip list that do not allow login');
INSERT INTO `rojcpp`.`options` (`id`, `created_at`, `updated_at`, `key`, `value`, `comment`) VALUES (3, DEFAULT, DEFAULT, 'allow_login_ips', '{}', 'the ip list that which can login; if not empty, it will be checked first');
INSERT INTO `rojcpp`.`options` (`id`, `created_at`, `updated_at`, `key`, `value`, `comment`) VALUES (4, DEFAULT, DEFAULT, 'deny_visit_ips', '{}', 'the ip list that do not allow view any page. You may deny yourself, be careful!');
INSERT INTO `rojcpp`.`options` (`id`, `created_at`, `updated_at`, `key`, `value`, `comment`) VALUES (5, DEFAULT, DEFAULT, 'allow_visit_ips', '{}', 'the ip list that can view page; if not empty, it will be checked first; You may deny yourself, be careful!');

COMMIT;

