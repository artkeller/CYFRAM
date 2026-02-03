# CRA Voluntary Full Compliance – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

**Purpose / Disclaimer:**  
This document outlines **voluntary full CRA compliance** measures for the CYFRAM Arduino Library.  
The library is **not commercially distributed**; all actions are **voluntary**, aimed at **audit-readiness, security mindset, and compliance training**.

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

# CRA Freiwillige Voll-Compliance – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇩🇪%20Deutsch-informational)](./LANGUAGE.md)

**Zweck / Hinweis:**  
Dieses Dokument beschreibt **freiwillige Voll-Compliance** der CYFRAM Arduino Library nach der EU-Cyber-Resilience-Verordnung (Verordnung (EU) 2024/2847).  
Die Library wird **nicht kommerziell vertrieben**; alle Maßnahmen sind **freiwillig**, mit Fokus auf **Audit-Tauglichkeit, Security-Mindset und Trainingszwecke**.

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
