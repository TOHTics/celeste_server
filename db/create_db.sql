-- MySQL Workbench Synchronization
-- Generated: 2017-09-01 15:19
-- Model: Solar Plant Database basic model
-- Version: 1.0
-- Project: Solar Plant Monitoring System
-- Author: Carlos Brito

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 ;

CREATE TABLE IF NOT EXISTS `mydb`.`Device` (
  `id` INT(11) NOT NULL,
  `Client_id` INT(11) NOT NULL,
  `Inventory_id` INT(11) NOT NULL,
  PRIMARY KEY (`Client_id`, `id`),
  INDEX `fk_Device_Client_idx` (`Client_id` ASC),
  INDEX `fk_Device_Inventory1_idx` (`Inventory_id` ASC),
  CONSTRAINT `fk_Device_Client`
    FOREIGN KEY (`Client_id`)
    REFERENCES `mydb`.`Client` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Device_Inventory1`
    FOREIGN KEY (`Inventory_id`)
    REFERENCES `mydb`.`Inventory` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`Client` (
  `id` INT(11) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`Inventory` (
  `id` INT(11) NOT NULL,
  `description` VARCHAR(45) NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`Point` (
  `type` VARCHAR(45) NOT NULL DEFAULT 'string',
  `Device_id` INT(11) NOT NULL,
  `Client_id` INT(11) NOT NULL,
  `id` VARCHAR(45) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Client_id`, `Device_id`, `Model_id`, `id`),
  UNIQUE INDEX `type_UNIQUE` (`type` ASC),
  INDEX `fk_Point_Model1_idx` (`Client_id` ASC, `Device_id` ASC, `Model_id` ASC),
  CONSTRAINT `fk_Point_Model1`
    FOREIGN KEY (`Client_id`, `Device_id` , `Model_id`)
    REFERENCES `mydb`.`Model` (`Client_id`, `Device_id` , `id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`Model` (
  `Device_id` INT(11) NOT NULL,
  `Client_id` INT(11) NOT NULL,
  `id` VARCHAR(45) NOT NULL,
  `Inventory_id` INT(11) NOT NULL,
  PRIMARY KEY (`Client_id`, `Device_id`, `id`),
  INDEX `fk_Model_Inventory1_idx` (`Inventory_id` ASC),
  INDEX `fk_Model_Device1_idx` (`Client_id`, `Device_id`),
  CONSTRAINT `fk_Model_Device1`
    FOREIGN KEY (`Client_id`, `Device_id`)
    REFERENCES `mydb`.`Device` (`Client_id`, `id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Model_Inventory1`
    FOREIGN KEY (`Inventory_id`)
    REFERENCES `mydb`.`Inventory` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`DeviceRecord` (
  `Device_id` INT(11) NOT NULL,
  `Client_id` INT(11) NOT NULL,
  `index` INT(11) NOT NULL,
  INDEX `fk_DeviceRecord_Device1_idx` (`Client_id` ASC, `Device_id` ASC),
  INDEX `fk_DeviceRecord_ModelRecord1_idx` (`index` ASC),
  PRIMARY KEY (`Client_id`, `Device_id`, `index`),
  CONSTRAINT `fk_DeviceRecord_Device1`
    FOREIGN KEY (`Client_id`, `Device_id`)
    REFERENCES `mydb`.`Device` (`Client_id`, `id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`ModelRecord` (
  `index` INT(11) NOT NULL,
  `Device_id` INT(11) NOT NULL,
  `Client_id` INT(11) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  `DeviceRecord_index` INT(11) NOT NULL,
  PRIMARY KEY (`Client_id`, `Device_id`, `Model_id`, `DeviceRecord_index`, `index`),
  INDEX `fk_ModelRecord_Model1_idx` (`Client_id` ASC, `Device_id` ASC, `Model_id` ASC),
  INDEX `fk_ModelRecord_DeviceRecord1_idx` (`DeviceRecord_index` ASC),
  INDEX `fk_ModelRecord_PointRecord1_idx` (`DeviceRecord_index` ASC, `index` ASC),
  CONSTRAINT `fk_ModelRecord_Model1`
    FOREIGN KEY (`Client_id`, `Device_id` , `Model_id`)
    REFERENCES `mydb`.`Model` (`Client_id`, `Device_id` , `id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_ModelRecord_DeviceRecord1`
    FOREIGN KEY (`DeviceRecord_index`)
    REFERENCES `mydb`.`DeviceRecord` (`index`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;

CREATE TABLE IF NOT EXISTS `mydb`.`PointRecord` (
  `index` INT(11) NOT NULL,
  `ModelRecord_index` INT(11) NOT NULL,
  `Device_id` INT(11) NOT NULL,
  `Client_id` INT(11) NOT NULL,
  `Model_id` VARCHAR(45) NOT NULL,
  `Point_id` VARCHAR(45) NOT NULL,
  `DeviceRecord_index` INT(11) NOT NULL,
  PRIMARY KEY (`Client_id`, `Device_id`, `Model_id`, `Point_id`, `DeviceRecord_index`, `ModelRecord_index`, `index`),
  INDEX `fk_PointRecord_Point1_idx` (`Client_id` ASC, `Device_id` ASC, `Model_id` ASC, `Point_id` ASC),
  INDEX `fk_PointRecord_ModelRecord1_idx` (`DeviceRecord_index` ASC, `ModelRecord_index` ASC),
 CONSTRAINT `fk_PointRecord_Point1`
  FOREIGN KEY (`Client_id`, `Device_id`, `Model_id` , `Point_id`)
  REFERENCES `mydb`.`Point` (`Client_id`, `Device_id`, `Model_id`, `id`)
  ON DELETE NO ACTION
  ON UPDATE NO ACTION,
 CONSTRAINT `fk_PointRecord_ModelRecord1`
  FOREIGN KEY (`DeviceRecord_index` , `ModelRecord_index`)
  REFERENCES `mydb`.`ModelRecord` (`DeviceRecord_index` , `index`)
  ON DELETE NO ACTION
  ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
