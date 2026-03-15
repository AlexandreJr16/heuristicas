import pandas as pd

# Base
base = pd.read_csv("resultados_base.csv")

print("=== MANHATTAN (média sobre sementes) ===")
print(base[base.heuristica == "manhattan"][["expansoes_medias","tempo_medio_ms"]].mean())

print("\n=== FORMULA (média sobre sementes) ===")
print(base[base.heuristica == "formula"][["expansoes_medias","tempo_medio_ms"]].mean())

print("\n=== MEMORY (média sobre sementes, agrupado por pivôs) ===")
print(base[base.heuristica == "memory"].groupby("n_pivos")[["preproc_ms","expansoes_medias","tempo_medio_ms"]].mean().round(2))

# Robustez
rob = pd.read_csv("resultados_robustez.csv")

print("\n=== ROBUSTEZ — degradação de expansões (média sobre sementes e pivôs) ===")
print(rob.groupby("n_bloqueios")[["degrad_exp_manhattan_pct","degrad_exp_memory_pct","degrad_exp_formula_pct"]].mean().round(2))

print("\n=== ROBUSTEZ — degradação de caminho fórmula (média sobre sementes e pivôs) ===")
print(rob.groupby("n_bloqueios")[["degrad_caminho_formula_pct"]].mean().round(2))

# Ratio
ratio = pd.read_csv("resultados_ratio.csv")
print("\n=== RATIO FÓRMULA (média sobre sementes) ===")
print(ratio.ratio_medio.mean().round(4))