# Dockerfile to build Journal Web App
#
# In production this is run by a `docker` container `journal.service` (systemd),
# and for development this is run by a `docker-compose` service `journal`.
#
# NOTE: (VS Code Dev Containers are provided (`Dev Containers: Reopen in Container`).
#
#   https://github.com/eulersson/taconez/blob/main/docs/5-configuring-your-ide.md#visual-studio-code
#
# NOTE: (non-VS Code users) Scripts that wrap the `docker build` and `docker run`:
#
#   ./modules/journal/docker/build-dev.sh
#   ./modules/journal/docker/run-dev.sh
#
# Adapted from: https://github.com/vercel/next.js/blob/canary/examples/with-docker/Dockerfile

FROM node:20.11.0-bookworm-slim AS base

# Global variables (across all stages).
ENV NEXT_TELEMETRY_DISABLED 1

WORKDIR /app/journal

RUN apt-get update && apt-get install -y git

# Install dependencies only when needed
FROM base AS deps

# Install dependencies based on the preferred package manager
COPY modules/journal/package.json modules/journal/package-lock.json ./
RUN npm ci

# Rebuild the source code only when needed
FROM base AS builder

COPY --from=deps /app/journal/node_modules ./node_modules
COPY ./modules/journal/ .

# IMPORTANT: Needs to be at the end to work well with Dev Containers. When Visual Studio
# Code starts building the containers it does take into account the build.target option
# in the docker-compose file, so it will always build the last stage in the Dockerfile
# (the one that is later in the file in terms of line number).
FROM builder AS development

ENTRYPOINT npm run dev

# Production image, copy all the files and run next
FROM base AS production

RUN npm run build

ENV NODE_ENV production

RUN addgroup --system --gid 1001 nodejs
RUN adduser --system --uid 1001 nextjs

COPY --from=builder /app/journal/public ./public

# Set the correct permission for prerender cache
RUN mkdir .next
RUN chown nextjs:nodejs .next

RUN apt apt-get purge -y git && apt-get autoremove --purge -y;

# Automatically leverage output traces to reduce image size
# https://nextjs.org/docs/advanced-features/output-file-tracing
COPY --from=builder --chown=nextjs:nodejs /app/journal/.next/standalone ./
COPY --from=builder --chown=nextjs:nodejs /app/journal/.next/static ./.next/static

USER nextjs

EXPOSE 3000
ENV PORT 3000

# Set hostname to localhost.
ENV HOSTNAME "0.0.0.0"

# server.js is created by next build from the standalone output
# https://nextjs.org/docs/pages/api-reference/next-config-js/output
CMD ["node", "server.js"]