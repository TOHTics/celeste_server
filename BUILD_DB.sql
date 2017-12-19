-- Version: 1.0
-- Project: CelesteDB
-- Author: Carlos Brito

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `Celeste` DEFAULT CHARACTER SET utf8 ;

CREATE TABLE IF NOT EXISTS `Celeste`.`Device` (
  `id` VARCHAR(45) NOT NULL,
  `man` VARCHAR(45) NOT NULL,
  `mod` VARCHAR(45) NOT NULL,
  `sn` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `sn_UNIQUE` (`sn` ASC))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `Celeste`.`Model` (
  `id` VARCHAR(45) NOT NULL,
  `ns` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `Celeste`.`Point` (
  `id` VARCHAR(45) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  `type` INT(11) NOT NULL DEFAULT 0,
  `u` VARCHAR(45) NULL DEFAULT NULL,
  `d` VARCHAR(100) NULL DEFAULT NULL,
  PRIMARY KEY (`id`, `Model_id`),
  INDEX `fk_Point_Model1_idx` (`Model_id` ASC),
  CONSTRAINT `fk_Point_Model1`
    FOREIGN KEY (`Model_id`)
    REFERENCES `Celeste`.`Model` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `Celeste`.`DeviceRecord` (
  `idx` INT(11) NOT NULL DEFAULT 0,
  `Device_id` VARCHAR(45) NOT NULL,
  `t` TIMESTAMP NOT NULL,
  `cid` VARCHAR(45) NULL DEFAULT NULL,
  `if` VARCHAR(45) NULL DEFAULT NULL,
  `lid` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`idx`, `Device_id`),
  INDEX `fk_DeviceRecord_Device1_idx` (`Device_id` ASC),
  CONSTRAINT `fk_DeviceRecord_Device1`
    FOREIGN KEY (`Device_id`)
    REFERENCES `Celeste`.`Device` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `Celeste`.`ModelRecord` (
  `Model_idx` INT(11) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  `DeviceRecord_idx` INT(11) NOT NULL,
  `Device_id` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Model_idx`, `Model_id`, `DeviceRecord_idx`, `Device_id`),
  INDEX `fk_ModelRecord_Model1_idx` (`Model_id` ASC),
  INDEX `fk_ModelRecord_DeviceRecord1_idx` (`DeviceRecord_idx` ASC, `Device_id` ASC),
  CONSTRAINT `fk_ModelRecord_Device_Model1`
    FOREIGN KEY (`Model_idx`)
    REFERENCES `Celeste`.`Device_Model` (`idx`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_ModelRecord_Model1`
    FOREIGN KEY (`Model_id`)
    REFERENCES `Celeste`.`Model` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_ModelRecord_DeviceRecord1`
    FOREIGN KEY (`DeviceRecord_idx` , `Device_id`)
    REFERENCES `Celeste`.`DeviceRecord` (`idx` , `Device_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `Celeste`.`PointRecord` (
  `idx` INT(11) NOT NULL,
  `Point_id` VARCHAR(45) NOT NULL,
  `Model_idx` INT(11) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  `DeviceRecord_idx` INT(11) NOT NULL,
  `Device_id` VARCHAR(45) NOT NULL,
  `sf` DOUBLE NULL DEFAULT NULL,
  `t` TIMESTAMP NULL DEFAULT NULL,
  `data` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`idx`, `Point_id`, `Model_idx`, `Model_id`, `DeviceRecord_idx`, `Device_id`),
  INDEX `fk_PointRecord_Point1_idx` (`Point_id` ASC),
  INDEX `fk_PointRecord_ModelRecord1_idx` (`Model_idx` ASC, `Model_id` ASC, `DeviceRecord_idx` ASC, `Device_id` ASC),
  CONSTRAINT `fk_PointRecord_Point1`
    FOREIGN KEY (`Point_id`)
    REFERENCES `Celeste`.`Point` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_PointRecord_ModelRecord1`
    FOREIGN KEY (`Model_idx` , `Model_id` , `DeviceRecord_idx` , `Device_id`)
    REFERENCES `Celeste`.`ModelRecord` (`Model_idx` , `Model_id` , `DeviceRecord_idx` , `Device_id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `Celeste`.`Device_Model` (
  `idx` INT(11) NOT NULL DEFAULT 0,
  `Device_id` VARCHAR(45) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  `note` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`idx`, `Device_id`, `Model_id`),
  INDEX `fk_Device_Model_Model1_idx` (`Model_id` ASC),
  INDEX `fk_Device_Model_Device1_idx` (`Device_id` ASC),
  CONSTRAINT `fk_Device_Model_Model1`
    FOREIGN KEY (`Model_id`)
    REFERENCES `Celeste`.`Model` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Device_Model_Device1`
    FOREIGN KEY (`Device_id`)
    REFERENCES `Celeste`.`Device` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
