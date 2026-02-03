# CRA Voluntary Full Compliance – CYFRAM Arduino Library

[![Language Selector](https://img.shields.io/badge/Select%20Language-%F0%9F%8C%90-informational?style=flat-square&logo=earth-africa)](./LANGUAGE.md)

**Short Summary / Disclaimer**  
This document demonstrates **voluntary full compliance** with the **EU Cyber Resilience Act (Regulation (EU) 2024/2847)** for the CYFRAM Arduino Library.  
The library is **not commercially distributed**. All listed measures are **voluntary**, taken for  
- audit-readiness  
- security awareness  
- training & learning purposes  

The document is available in four languages:

- 🇬🇧 **[English](#english)** – Hauptversion / main version  
- 🇩🇪 **[Deutsch](#deutsch)**  
- 🇯🇵 **[日本語](#日本語)**  
- 🇨🇳 **[简体中文](#简体中文)**

---

### English

**Purpose / Disclaimer**  
This document outlines **voluntary full CRA compliance** measures for the CYFRAM Arduino Library. The library is **not commercially distributed**; all actions are **voluntary**, aimed at **audit-readiness, security mindset, and compliance training**.

---

## 1. Secure by Design

- Input validation for I²C/SPI addresses and buffers  
- Error codes documented (NACK, overflow, null pointer)  
- Threat modeling planned for I²C/SPI  
- Guidelines for safe usage documented

---

## 2. Security Updates & Patch Management

- SECURITY.md defines reporting process (voluntary)  
- Version tracking for affected releases  
- Optional: cryptographic verification / checksums for releases

---

## 3. Documentation & Guidance

- LANGUAGE.md & CODESTYLE.md for consistent code quality  
- Integration notes for downstream use  
- Known limitations documented

---

## 4. SBOM & Dependencies

- All external dependencies tracked (Wire.h, SPI.h)  
- SPDX identifiers, versions, and licenses recorded  
- Monitoring of vulnerabilities recommended

---

## 5. Testing & Audit Readiness

- Unit tests for edge cases  
- Integration tests on I²C/SPI hardware  
- Fuzzing / stress testing  
- Documentation of test results

---

## 6. Versioning & Releases

- Semantic Versioning: v1.0.0 → v1.2.0  
- Tagged releases  
- Optional: signed builds / hashes

---

## 7. Downstream Guidance

- If integrated into a commercial product: manufacturer/distributor assumes CRA obligations  
- Minimum safe usage recommendations  
- Warnings for unsafe operations

---

## 8. References

- [EU Cyber Resilience Act – Open Source Guidance](https://digital-strategy.ec.europa.eu/en/policies/cra-open-source)  
- [Regulation (EU) 2024/2847](https://eur-lex.europa.eu/legal-content/EN/TXT/?uri=CELEX%3A32024R2847)

---

> This document demonstrates **voluntary full CRA compliance**, audit-readiness, and security awareness for learning and practice purposes.
---

### Deutsch

**Zweck / Hinweis**  
Dieses Dokument beschreibt **freiwillige Voll-Compliance** der CYFRAM Arduino Library nach der EU-Cyber-Resilience-Verordnung (Verordnung (EU) 2024/2847). Die Library wird **nicht kommerziell vertrieben**; alle Maßnahmen sind **freiwillig**, mit Fokus auf **Audit-Tauglichkeit, Security-Mindset und Trainingszwecke**.

---

## 1. Secure by Design

- Input-Validierung für I²C/SPI-Adressen und Puffer  
- Fehlercodes dokumentiert (NACK, Overflow, Nullpointer)  
- Threat Modeling für I²C/SPI geplant  
- Richtlinien für sichere Nutzung dokumentiert

---

## 2. Sicherheitsupdates & Patch-Management

- SECURITY.md definiert Meldungsprozess (freiwillig)  
- Versionstracking für betroffene Releases  
- Optional: kryptografische Signaturen / Prüfsummen für Releases

---

## 3. Dokumentation & Hinweise

- LANGUAGE.md & CODESTYLE.md für konsistente Codequalität  
- Hinweise für Downstream-Nutzung  
- Bekannte Limitationen dokumentiert

---

## 4. SBOM & Abhängigkeiten

- Alle externen Abhängigkeiten erfasst (Wire.h, SPI.h)  
- SPDX-IDs, Versionen und Lizenzen dokumentiert  
- Monitoring bekannter Schwachstellen empfohlen

---

## 5. Tests & Audit-Tauglichkeit

- Unit-Tests für Randfälle  
- Integrationstests auf I²C/SPI Hardware  
- Fuzzing / Stresstests  
- Testergebnisse dokumentiert

---

## 6. Versionierung & Releases

- Semantic Versioning: v1.0.0 → v1.2.0  
- Tagged Releases  
- Optional: signierte Builds / Prüfsummen

---

## 7. Hinweise für Downstream-Nutzer

- Bei Integration in kommerzielle Produkte → Hersteller/Vertreiber übernimmt CRA-Pflichten  
- Minimale Nutzungsempfehlungen / Hardware-Voraussetzungen  
- Warnhinweise für unsichere Operationen

---

## 8. Referenzen

- [EU Cyber Resilience Act – Open Source Guidance](https://digital-strategy.ec.europa.eu/en/policies/cra-open-source)  
- [Verordnung (EU) 2024/2847](https://eur-lex.europa.eu/legal-content/DE/TXT/?uri=CELEX%3A32024R2847)

---

> Dieses Dokument demonstriert **freiwillige Voll-CRA-Compliance**, Audit-Tauglichkeit und Security Awareness für Lern- und Trainingszwecke.

---

# CRA 自主的な完全準拠 – CYFRAM ライブラリ

[![Language](https://img.shields.io/badge/language-🇯🇵%20日本語-informational)](./LANGUAGE.md)

**目的/注記：**
この文書は、EUサイバーレジリエンス規則（規則（EU）2024/2847）に基づく、CYFRAM Arduinoライブラリの**自主的な完全準拠**について記載しています。
このライブラリは**商業的に販売されていません**。すべての措置は **任意** であり、**監査対応性、セキュリティマインドセット、およびトレーニング目的** に重点を置いています。

---

## 1. 設計によるセキュリティ

- I²C/SPI アドレスおよびバッファの入力検証
- エラーコードの文書化 (NACK、オーバーフロー、ヌルポインタ)
 
- I²C/SPI の脅威モデリングを計画
- 安全な使用に関するガイドラインを文書化

---

## 2. セキュリティアップデートとパッチ管理

- SECURITY.md で報告プロセスを定義（任意）
- 影響を受けるリリースのバージョン追跡
- オプション：リリースの暗号署名/チェックサム

---

## 3. ドキュメントと注意事項

- LANGUAGE.md および CODESTYLE.md による一貫したコード品質
- ダウンストリーム利用に関する注意事項
- 既知の制限事項の文書化

---

## 4. SBOM および依存関係

- すべての外部依存関係（Wire.h、SPI.h）の登録 
- SPDX ID、バージョン、ライセンスを文書化
- 既知の脆弱性のモニタリングを推奨

---

## 5. テストと監査の適合性

- エッジケースのユニットテスト
- I²C/SPI ハードウェアの統合テスト
- ファジング/ストレステスト
- テスト結果を文書化

---

## 6. バージョン管理とリリース

- セマンティックバージョニング：v1.0.0 → v1.2.0
- タグ付きリリース
- オプション：署名付きビルド/チェックサム

---

## 7. ダウンストリームユーザー向け情報

- 商用製品への統合の場合 → メーカー/販売業者が CRA 義務を引き継ぐ 
- 最低限の使用推奨事項 / ハードウェア要件
- 安全でない操作に関する警告

---

## 8. 参考資料

- [EU サイバーレジリエンス法 – オープンソースガイダンス](https://digital-strategy.ec.europa.eu/en/policies/cra-open-source) 
- [規則 (EU) 2024/2847](https://eur-lex.europa.eu/legal-content/DE/TXT/?uri=CELEX%3A32024R2847)

---

> この文書は、学習および研修の目的で、**自主的な完全な CRA コンプライアンス**、監査対応性、およびセキュリティ意識を実証するものです。

---


# CRA 自愿完全合规——CYFRAM 库

[![Language](https://img.shields.io/badge/language-🇨🇳%20简体中文-informational)](./LANGUAGE.md)

**目的/说明：**
本文档描述了 CYFRAM Arduino 库根据欧盟网络弹性条例（条例 (EU) 2024/2847）的**自愿完全合规**情况。
该库**不进行商业销售**； 所有措施都是**自愿的**，侧重于**审计能力、安全思维和培训目的**。

---

## 1. 设计安全

- I²C/SPI 地址和缓冲区的输入验证
- 记录错误代码（NACK、溢出、空指针）
- 计划对 I²C/SPI 进行威胁建模
- 记录安全使用准则

---

## 2. 安全更新和补丁管理

- SECURITY.md 定义了报告流程（自愿）
- 相关版本的版本跟踪
- 可选：版本的加密签名/校验和

---

## 3. 文档与说明

- LANGUAGE.md 与 CODESTYLE.md 确保代码质量的一致性
- 下游使用说明
- 已知限制记录

---

## 4. SBOM 与依赖关系

- 记录所有外部依赖关系（Wire.h、SPI.h）
- 记录 SPDX ID、版本和许可证
- 建议监控已知的弱点

---

## 5. 测试和审计适用性

- 边缘情况的单元测试
- I²C/SPI 硬件上的集成测试
- 模糊测试/压力测试
- 记录测试结果

---

## 6. 版本控制和发布

- 语义版本控制：v1.0.0 → v1.2.0
- 标记版本
- 可选：签名构建/校验和

---

## 7. 下游用户注意事项

- 集成到商业产品中时→制造商/经销商承担 CRA 义务
- 最低使用建议/硬件要求
- 不安全操作的警告

---

## 8. 参考资料

- [欧盟网络弹性法案——开源指南](https://digital-strategy.ec.europa.eu/en/policies/cra-open-source)
- [欧盟条例 (EU) 2024/2847](https://eur-lex.europa.eu/legal-content/DE/TXT/?uri=CELEX%3A32024R2847)

---

> 本文件展示了**自愿的全面CRA合规性**、可审计性和安全意识，用于学习和培训目的。
