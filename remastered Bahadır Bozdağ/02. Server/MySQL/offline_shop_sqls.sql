/*
 Navicat Premium Data Transfer

 Source Server         : 185.126.178.242
 Source Server Type    : MySQL
 Source Server Version : 50562
 Source Host           : 185.126.178.242:3306
 Source Schema         : player

 Target Server Type    : MySQL
 Target Server Version : 50562
 File Encoding         : 65001

 Date: 02/04/2021 17:35:38
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for offline_shop_item
-- ----------------------------
DROP TABLE IF EXISTS `offline_shop_item`;
CREATE TABLE `offline_shop_item`  (
  `id` int(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  `owner_id` int(11) UNSIGNED NOT NULL DEFAULT 0,
  `pos` smallint(5) UNSIGNED NOT NULL DEFAULT 0,
  `count` tinyint(3) UNSIGNED NOT NULL DEFAULT 0,
  `vnum` int(11) UNSIGNED NOT NULL DEFAULT 0,
  `price` int(11) NOT NULL DEFAULT 0,
  `price2` tinyint(3) NOT NULL DEFAULT 0,
  `price_type` tinyint(1) NOT NULL DEFAULT 0,
  `evolution` tinyint(1) UNSIGNED NOT NULL DEFAULT 0,
  `sold` tinyint(1) NOT NULL DEFAULT 0,
  `buyer_name` varchar(64) CHARACTER SET latin5 COLLATE latin5_turkish_ci NOT NULL,
  `socket0` int(10) UNSIGNED NOT NULL DEFAULT 0,
  `socket1` int(10) UNSIGNED NOT NULL DEFAULT 0,
  `socket2` int(10) UNSIGNED NOT NULL DEFAULT 0,
  `socket3` int(10) UNSIGNED NOT NULL DEFAULT 0,
  `attrtype0` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue0` smallint(6) NOT NULL DEFAULT 0,
  `attrtype1` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue1` smallint(6) NOT NULL DEFAULT 0,
  `attrtype2` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue2` smallint(6) NOT NULL DEFAULT 0,
  `attrtype3` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue3` smallint(6) NOT NULL DEFAULT 0,
  `attrtype4` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue4` smallint(6) NOT NULL DEFAULT 0,
  `attrtype5` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue5` smallint(6) NOT NULL DEFAULT 0,
  `attrtype6` tinyint(4) NOT NULL DEFAULT 0,
  `attrvalue6` smallint(6) NOT NULL DEFAULT 0,
  `item_name` varbinary(64) NOT NULL DEFAULT 'WORLD',
  `shop_id` int(11) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `owner_id_idx`(`owner_id`) USING BTREE,
  INDEX `item_vnum_index`(`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 10003065 CHARACTER SET = latin5 COLLATE = latin5_turkish_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for offline_shop_npc
-- ----------------------------
DROP TABLE IF EXISTS `offline_shop_npc`;
CREATE TABLE `offline_shop_npc`  (
  `owner_id` int(11) NOT NULL DEFAULT 0,
  `account_id` int(11) NOT NULL DEFAULT 0,
  `sign` varchar(32) CHARACTER SET latin5 COLLATE latin5_turkish_ci NOT NULL,
  `name` varchar(32) CHARACTER SET latin5 COLLATE latin5_turkish_ci NULL DEFAULT NULL,
  `time` int(11) NULL DEFAULT NULL,
  `x` int(11) NULL DEFAULT NULL,
  `y` int(11) NULL DEFAULT NULL,
  `z` int(11) NULL DEFAULT NULL,
  `mapIndex` int(11) NULL DEFAULT NULL,
  `channel` int(2) NULL DEFAULT NULL,
  `level` tinyint(3) NULL DEFAULT NULL,
  `align` int(11) NULL DEFAULT NULL,
  `guild` varchar(255) CHARACTER SET latin5 COLLATE latin5_turkish_ci NULL DEFAULT 'NONAME',
  PRIMARY KEY (`owner_id`) USING BTREE
) ENGINE = MyISAM CHARACTER SET = latin5 COLLATE = latin5_turkish_ci ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;
