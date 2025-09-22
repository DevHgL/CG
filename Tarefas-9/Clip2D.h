template<class Vertex>
bool line_clip(Line<Vertex>& line){
    Vertex v0 = line[0];
    Vertex v1 = line[1];

    vec2 P = get2DPosition(v0);
    vec2 Q = get2DPosition(v1);

    float t0 = 0.0f;
    float t1 = 1.0f;

    vec2 d = Q - P;

    for(const Semiplane& S : clip_polygon){
        float num = dot(S.A - P, S.n);
        float den = dot(d, S.n);

        if (fabs(den) < 1e-6f) {
            // Linha paralela
            if (num < 0) return false; // fora de vez
            else continue;             // dentro, segue
        }

        float t = num / den;

        if (den < 0) {
            // entrando
            if (t > t0) t0 = t;
        } else {
            // saindo
            if (t < t1) t1 = t;
        }

        if (t0 > t1) return false; // totalmente fora
    }

    if (t0 > 0) line[0] = mix_line(t0, Line<Vertex>{v0, v1});
    if (t1 < 1) line[1] = mix_line(t1, Line<Vertex>{v0, v1});

    return true;
}
