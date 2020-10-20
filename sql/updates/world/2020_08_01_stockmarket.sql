SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for stocks
-- ----------------------------
DROP TABLE IF EXISTS `stocks`;
CREATE TABLE `stocks` (
  `iD` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Creature Identifier',
  `Scale` int(10) unsigned NOT NULL DEFAULT '120',
  `OwnedRatio` float NOT NULL DEFAULT '0',
  `Worth` int(10) unsigned NOT NULL DEFAULT '120' COMMENT 'in gold',
  `UpdateTime` int(10) NOT NULL,
  `comments` text NOT NULL,
  PRIMARY KEY (`iD`),
  KEY `idx_id` (`iD`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Creature System';

-- ----------------------------
-- Records of stocks
-- ----------------------------
INSERT INTO `stocks` VALUES ('0', '1', '0', '10', '18542578', '');
INSERT INTO `stocks` VALUES ('1', '1', '0', '10', '0', '太阳之井');
INSERT INTO `stocks` VALUES ('2', '1', '0', '10', '0', '黑暗神庙');
INSERT INTO `stocks` VALUES ('3', '1', '0', '10', '0', '盘牙水库');
INSERT INTO `stocks` VALUES ('4', '1', '0', '10', '0', '海加尔山');
INSERT INTO `stocks` VALUES ('5', '1', '0', '10', '0', '风暴要塞');