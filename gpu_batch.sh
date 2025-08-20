#!/bin/bash
#SBATCH --partition=gpuws
#SBATCH --gres=gpu:1
#SBATCH --time=00:05:00

if [ -z "${PARAM}" ]; then
        PARAM="params.ini"
fi

module purge
module load nvhpc/25.7

cd ${SLURM_SUBMIT_DIR}

FILENAME="$(date +%Y%m%d)_$(date +%H%M).log"

./bin/collapse < ${PARAM} > output/${FILENAME}
