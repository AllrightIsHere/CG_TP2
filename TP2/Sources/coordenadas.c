#include "../Headers/coordenadas.h"
#include <math.h>
#include <stdio.h>

/// multiplica um vetor por um escalar
/// este é um exemplo entregue pronto pra você ;)
vetor multiplicaPorEscalar(vetor v, escalar alpha)
{
    vetor resultado =
    {
        v.x * alpha,
        v.y * alpha,
        v.z * alpha,
        v.w * alpha
    };
    return resultado;
}

vetor somaVetorComVetor(vetor v, vetor u)
{
    //TODO: implementar
    vetor resultado = v;
    resultado.x = v.x + u.x;
    resultado.y = v.y + u.y;
    resultado.z = v.z + u.z;
    return resultado;
}

vetor diferencaVetorComVetor(vetor v, vetor u)
{
    //TODO: implementar
    vetor resultado = v;
    resultado.x = v.x - u.x;
    resultado.y = v.y - u.y;
    resultado.z = v.z - u.z;
    return resultado;
}

vetor diferencaEntrePontos(ponto p, ponto q)
{
    //TODO: implementar
    vetor resultado = {0,0,0,0};
    resultado.x = p.x - q.x;
    resultado.y = p.y - q.y;
    resultado.z = p.z - q.z;
    return resultado;
}

ponto somaPontoComVetor(ponto p, vetor v)
{
    //TODO: implementar
    ponto resultado = p;
    resultado.x = p.x + v.x;
    resultado.y = p.y + v.y;
    resultado.z = p.z + v.z;
    return resultado;
}

escalar normaDoVetor(vetor v)
{
    //TODO: implementar
    escalar resultado = 0;
    resultado = sqrt(produtoEscalar(v, v));
    return resultado;
}

vetor normalizado(vetor v)
{
    //TODO: implementar
    vetor resultado = v;
    resultado.x = v.x / normaDoVetor(v);
    resultado.y = v.y / normaDoVetor(v);
    resultado.z = v.z / normaDoVetor(v);
    return resultado;
}

escalar distanciaEntrePontos(ponto p, ponto q)
{
    //TODO: implementar
    escalar resultado = 0;
    resultado = sqrt(pow(p.x - q.x, 2) + pow(p.y - q.y, 2) + pow(p.z - q.z, 2));
    return resultado;
}

escalar produtoEscalar(vetor v, vetor u)
{
    //TODO: implementar
    escalar resultado = 1;
    resultado = (v.x * u.x) + (v.y * u.y) + (v.z * u.z);
    return resultado;
}

vetor produtoVetorial(vetor v, vetor u)
{
    //TODO: implementar
    // Produto vetorial só faz sentido em 3D
    // Ignorar a componente "w" de "v" e "u"
    // Como o resultado é um vetor, o "w" dele deve ser 0
    vetor resultado = v;
    resultado.x = (v.y * u.z) - (v.z * u.y);
    resultado.y = (v.z * u.x) - (v.x * u.z);
    resultado.z = (v.x * u.y) - (v.y * u.x);
    return resultado;
}

///
/// Referências: http://localhost:8080/classes/geometry/#30
escalar anguloEntreVetores(vetor v, vetor u)
{
    //TODO: implementar
    escalar resultado = 0;
    resultado = acos(produtoEscalar(v, u) / (normaDoVetor(v) * normaDoVetor(u)));
    return resultado;
}

///
/// Referências: http://localhost:8080/classes/geometry/#22
ponto combinacaoAfim2Pontos(ponto p, ponto q, escalar alpha)
{
    //TODO: implementar
    ponto resultado = p;
    resultado = somaPontoComVetor(q, multiplicaPorEscalar(diferencaEntrePontos(p, q), alpha));
    return resultado;
}

/// Imprime um vetor ou ponto no terminal
/// Uso:
///   vetor r = somaVetorComVetor(a, b);
///   imprime("vetor r", r);
void imprime(struct coordenadas c, char* nome)
{
    printf("%s = { %.2f, %.2f, %.2f, %.2f }\n", nome, c.x, c.y, c.z, c.w);
}
