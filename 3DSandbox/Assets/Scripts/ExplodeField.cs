using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExplodeField : MonoBehaviour
{
    public GameObject Explosion;
    private bool hasExploded = false;
    [SerializeField]
    private float explodeBeforeTimer = 0f;
    [SerializeField][Header("��ըǰҡ��ʱ")]
    private float explodeBeforeTime = 2f;

    [SerializeField][Header("��ը��")]
    private float explodeForce = 100f;

    [SerializeField][Header("��ը�뾶")]
    private float explodeRadius = 10f;

    [SerializeField][Header("��ը��ҡ��ʱ")]
    private float explodeAfterTimer = 0f;
    [SerializeField][Header("��ը��ҡ")]
    private float explodeAfterTime = 1f;
    private void OnTriggerEnter(Collider other)
    {
        if (GetComponent<SphereCollider>().enabled == false)
            return;
        if (other.tag == "Cube")
        {
            Vector3 dir = other.transform.position - transform.position;
            other.GetComponent<Rigidbody>().AddForce(dir.normalized * explodeForce);
        }
    }
    
    private void Update()
    {
        if (enabled == false)
            return;
        if (!hasExploded)
            explodeBeforeTimer += Time.deltaTime;
        else
        {
            explodeAfterTimer += Time.deltaTime; 
            if (explodeAfterTimer >= explodeAfterTime)
                Destroy(gameObject);
            return;
        }
        if (explodeBeforeTimer >= explodeBeforeTime)
            Explode();
    }
    void Explode()
    {
        hasExploded = true;
        GetComponent<SphereCollider>().radius = explodeRadius;
        GetComponent<SphereCollider>().enabled = true;
        //Explosion.SetActive(true);
    }
}
