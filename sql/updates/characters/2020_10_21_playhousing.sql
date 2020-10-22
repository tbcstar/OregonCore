-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.5.9 - MySQL Community Server (GPL)
-- Server OS:                    Win32
-- HeidiSQL Version:             9.4.0.5125
-- --------------------------------------------------------
 
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
 
 
-- Dumping database structure for characters
CREATE DATABASE IF NOT EXISTS `characters` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `characters`;
 
-- Dumping structure for table characters.player_house
CREATE TABLE IF NOT EXISTS `player_house` (
  `player_guid` int(11) NOT NULL,
  `player_name` varchar(36) NOT NULL,
  `items_spawned` int(11) unsigned DEFAULT '0',
  `npcs_spawned` int(11) unsigned DEFAULT '0',
  `items_bonus` int(11) DEFAULT '0',
  `npcs_bonus` int(11) DEFAULT '0',
  `package1` int(11) DEFAULT NULL,
  `package2` int(11) DEFAULT NULL,
  `package3` int(11) DEFAULT NULL,
  `package4` int(11) DEFAULT NULL,
  PRIMARY KEY (`player_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 
-- Dumping structure for table characters.player_house_available
CREATE TABLE IF NOT EXISTS `player_house_available` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `zone_name` text,
  `price` int(11) NOT NULL DEFAULT '15',
  `map_id` int(11) DEFAULT NULL,
  `bottom_x` float DEFAULT NULL,
  `top_x` float DEFAULT NULL,
  `left_y` float DEFAULT NULL,
  `right_y` float DEFAULT NULL,
  `spawn_x` float DEFAULT NULL,
  `spawn_y` float DEFAULT NULL,
  `spawn_z` float DEFAULT NULL,
  `spawn_o` float DEFAULT NULL,
  `c_guid` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;
 
-- Dumping structure for table characters.player_house_npcs
CREATE TABLE IF NOT EXISTS `player_house_npcs` (
  `npcId` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`npcId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- Dumping data for table characters.player_house_npcs: 0 rows
/*!40000 ALTER TABLE `player_house_npcs` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_house_npcs` ENABLE KEYS */;
 
-- Dumping structure for table characters.player_house_objects
CREATE TABLE IF NOT EXISTS `player_house_objects` (
  `gobjID` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`gobjID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
 
-- Dumping data for table characters.player_house_objects: 0 rows
/*!40000 ALTER TABLE `player_house_objects` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_house_objects` ENABLE KEYS */;
 
-- Dumping structure for table characters.player_house_spawns
CREATE TABLE IF NOT EXISTS `player_house_spawns` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `CreaOrObjId` int(10) unsigned NOT NULL DEFAULT '0',
  `CreaOrObjName` varchar(50) DEFAULT '0',
  `CreatureOrObject` int(3) unsigned NOT NULL DEFAULT '0',
  `package` int(3) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 
-- Dumping data for table characters.player_house_spawns: ~1 rows (approximately)
/*!40000 ALTER TABLE `player_house_spawns` DISABLE KEYS */;
INSERT INTO `player_house_spawns` (`id`, `CreaOrObjId`, `CreaOrObjName`, `CreatureOrObject`, `package`) VALUES
    (1, 31395, '伊利丹·怒风', 0, 1),
    (1, 10217, '椅子', 1, 1),
    (2, 193167, '双层床', 1, 1),
    (3, 191966, '桌子', 1, 1),
    (4, 181103, '花儿', 1, 1),
    (1, 193167, '双层床', 1, 2),
    (1, 31395, '伊利丹·怒风', 0, 2),
    (1, 193167, '双层床', 1, 3),
    (1, 31395, '伊利丹·怒风', 0, 3),
    (1, 193167, '双层床', 1, 4),
    (1, 31395, '伊利丹·怒风', 0, 4),
    (1, 193167, '双层床', 1, 5),
    (1, 31395, '伊利丹·怒风', 0, 5),
    (2, 31395, '不是伊利丹·怒风', 0, 1);
/*!40000 ALTER TABLE `player_house_spawns` ENABLE KEYS */;
 
-- Dumping structure for table characters.player_house_taken
CREATE TABLE IF NOT EXISTS `player_house_taken` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `player_name` varchar(12) DEFAULT NULL,
  `player_guid` int(11) unsigned DEFAULT '0',
  `zone_name` text,
  `map_id` int(11) DEFAULT NULL,
  `bottom_x` float DEFAULT NULL,
  `top_x` float DEFAULT NULL,
  `left_y` float DEFAULT NULL,
  `right_y` float DEFAULT NULL,
  `spawn_x` float DEFAULT NULL,
  `spawn_y` float DEFAULT NULL,
  `spawn_z` float DEFAULT NULL,
  `spawn_o` float DEFAULT NULL,
  `sign1x` float DEFAULT NULL,
  `sign1y` float DEFAULT NULL,
  `sign2x` float DEFAULT NULL,
  `sign2y` float DEFAULT NULL,
  `sign3x` float DEFAULT NULL,
  `sign3y` float DEFAULT NULL,
  `sign4x` float DEFAULT NULL,
  `sign4y` float DEFAULT NULL,
  `upgrades` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;