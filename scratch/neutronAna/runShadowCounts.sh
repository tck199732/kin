#!/bin/bash

result_dir="results/shadow_no_veto"
mkdir -p $result_dir

for shadow in "before" "moved" "final"; do 
    root -b -q -l "macros/shadow_bar_studies/DrawHitPattern.C(\"$shadow\", \"${result_dir}/hit_pattern_${shadow}.root\", false)"
done

for shadow in "before" "moved" "final"; do 
    root -b -q -l "macros/shadow_bar_studies/DrawBarHitPattern.C(\"$shadow\", \"${result_dir}/bar_hit_pattern_${shadow}.root\", false)"
done

for shadow in "before" "moved" "final"; do 
    root -b -q -l "macros/shadow_bar_studies/DrawShadowCounts.C(\"$shadow\", {0, 1000}, false, \"${result_dir}/shadow_counts_${shadow}.root\", 200)"
done

result_dir="results/shadow_vetoed"
mkdir -p $result_dir

for shadow in "before" "moved" "final"; do 
    root -b -q -l "macros/shadow_bar_studies/DrawHitPattern.C(\"$shadow\", \"${result_dir}/hit_pattern_${shadow}.root\", true)"
done

for shadow in "before" "moved" "final"; do 
    root -b -q -l "macros/shadow_bar_studies/DrawBarHitPattern.C(\"$shadow\", \"${result_dir}/bar_hit_pattern_${shadow}.root\", true)"
done

for shadow in "before" "moved" "final"; do 
    root -b -q -l "macros/shadow_bar_studies/DrawShadowCounts.C(\"$shadow\", {0, 1000}, true, \"${result_dir}/shadow_counts_${shadow}.root\", 200)"
done



