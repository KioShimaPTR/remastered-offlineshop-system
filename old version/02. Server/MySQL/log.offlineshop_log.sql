/*
Navicat MySQL Data Transfer

Source Server         : FinchServer
Source Server Version : 50553
Source Host           : 192.168.1.21:3306
Source Database       : log

Target Server Type    : MYSQL
Target Server Version : 50553
File Encoding         : 65001

Date: 2017-07-12 16:15:39
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for offlineshop_log
-- ----------------------------
DROP TABLE IF EXISTS `offlineshop_log`;
CREATE TABLE `offlineshop_log` (
  `account_id` int(11) NOT NULL DEFAULT '0',
  `action` varchar(64) CHARACTER SET latin1 NOT NULL,
  `item` varchar(24) NOT NULL,
  `time` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP
) ENGINE=MyISAM DEFAULT CHARSET=latin5;
